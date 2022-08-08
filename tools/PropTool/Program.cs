using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Threading;

namespace PropToolProgram
{
    public enum Load_Type
    {
        ToRAM = 1,
        ToEEPROM = 3,
    }

    //arguments
    //1: Port
    //2: flash destination (--eeprom or --ram), optional. if doesn't exit, flash to eeprom
    //3: Program to flash
    //4: leave port open and listen (--listen <baud>), optional. Default close port after uploading

    //examples
    //PropTool.exe --eeprom ./SpinProgram.binary
    //PropTool.exe --ram ./SpinProgram.binary --listen 115200
    static class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Cmd c = Cmd.Parse(args);

            if (c["eeprom"] == null && c["ram"] == null)
            {
                Console.WriteLine("Checking for propeller on port {0}", args[0]);
                PropTool p = new PropTool();
                p.OpenPort(args[0]);
                if (p.HwFind())
                    Console.WriteLine("OK!");
                else
                    Console.WriteLine("ERROR!");

                p.ClosePort();
                return;
            }
            else
            {
                Load_Type lt = (Load_Type)0;
                CmdOption co;

                if ((co = c["eeprom"]) != null)
                    lt = Load_Type.ToEEPROM;
                else if ((co = c["ram"]) != null)
                    lt = Load_Type.ToRAM;

                if (co == null || lt == 0)
                {
                    Console.WriteLine("Invalid argument");
                    return;
                }

                string prog = co.Value;

                if (string.IsNullOrEmpty(prog) || !File.Exists(prog))
                {
                    Console.WriteLine("Specified program does not exist");
                    return;
                }
                else
                {
                    Console.WriteLine("Loading program {0} to {1}", prog, co.Flag.ToUpper());

                    PropTool p = new PropTool();
                    p.OpenPort(args[0]);

                    bool loaded = p.LoadBinaryFile(prog, lt);

                    if (loaded)
                        Console.WriteLine("OK!");
                    else
                    {
                        Console.WriteLine("ERROR!");
                        return;
                    }

                    if (c["listen"] == null)
                        return;

                    int baud = int.Parse(c["listen"].Value);

                    p.ChangeBaudRate(baud);
                    p.Listen();

                    while (true)
                    {
                        Console.ReadKey();

                        p.ClosePort();
                        break;
                    }
                }
            }
        }
    }

    public class PropTool : IDisposable
    {
        private SerialPort port;
        private static int LFSR = 'P';
        private byte[] lfsrBuffer = new byte[500];
        private byte[] receiveBuffer = new byte[258];

        public PropTool()
        {
            for (int i = 0; i < 500; i++)
                lfsrBuffer[i] = (byte)(Iterate() | 0xFE);

            for (int i = 0; i < 258; i++)
                receiveBuffer[i] = 0xF9;
        }

        public void OpenPort(string p)
        {
            Console.WriteLine("Opening port");
            port = new SerialPort(p, 115200, Parity.None, 8, StopBits.One);
            port.ReadTimeout = 150;
            port.WriteTimeout = 150;
            port.RtsEnable = false;
            port.DtrEnable = false;

            if (!port.IsOpen)
                port.Open();
        }

        public void ChangeBaudRate(int baud)
        {
            if (!port.IsOpen)
                return;

            port.DiscardInBuffer();
            port.DiscardOutBuffer();
            port.BaudRate = baud;
        }

        public void Listen()
        {
            port.DataReceived += (s, e) =>
            {
                Console.Write(port.ReadExisting());
            };
        }

        public void ClosePort()
        {
            if (port.IsOpen)
            {
                port.DiscardInBuffer();
                port.DiscardOutBuffer();
                port.Close();
            }

            port.Dispose();
        }

        public bool HwFind()
        {
            try
            {
                Reset();

                port.Write(new byte[] { 0xF9 }, 0, 1);

                port.Write(lfsrBuffer, 0, 250);
                port.Write(receiveBuffer, 0, 258);

                WaitForBytes(258, port.ReadTimeout);
                port.Read(receiveBuffer, 0, 258);

                for (int i = 0; i < 250; i++)
                    if (receiveBuffer[i] != lfsrBuffer[i + 250])
                        return false;

                int version = 0;
                for (int i = 250; i < 258; i++)
                    version = ((version >> 1) & 0x7f) | ((receiveBuffer[i] & 0x01) << 7);

                if (version != 1)
                {
                    Console.WriteLine("Propeller not found");
                    return false;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }

            Console.WriteLine("Propeller found");
            return true;
        }

        private int Iterate()
        {
            int bit = LFSR & 1;
            LFSR = LFSR << 1 | (LFSR >> 7 ^ LFSR >> 5 ^ LFSR >> 4 ^ LFSR >> 1) & 1;
            return bit;
        }

        public void Reset()
        {
            Console.WriteLine("Resetting device");
            port.RtsEnable = port.DtrEnable = true;
            Thread.Sleep(50);
            port.RtsEnable = port.DtrEnable = false;
            Thread.Sleep(100);
        }

        private void WaitForBytes(int count, int millisecondsTimeout)
        {
            if (port.BytesToRead >= count)
                return;

            DateTime expire = DateTime.Now.AddMilliseconds(millisecondsTimeout);

            while (port.BytesToRead < count)
            {
                if (DateTime.Now >= expire)
                    throw new TimeoutException();

                Thread.Sleep(25);
            }
        }

        public bool LoadBinaryFile(string path, Load_Type type)
        {
            byte[] image = File.ReadAllBytes(path);

            if (HwFind())
            {
                if (Upload(image, type))
                    return true;
            }

            return false;
        }

        private bool Upload(byte[] image, Load_Type type)
        {
            SendLong((int)type);
            SendLong(image.Length / 4);

            for (int i = 0; i < image.Length; i += 4)
                SendLong(image[i] | (image[i + 1] << 8) | (image[i + 2] << 16) | (image[i + 3] << 24));

            Thread.Sleep(50);

            if (GetAck(100))
            {
                if (type == Load_Type.ToEEPROM)
                {
                    if (GetAck(500))
                    {
                        if (GetAck(500))
                            return true;
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else
                    return true;
            }

            return false;
        }

        private void SendLong(int data)
        {
            byte[] buf = new byte[11];

            for (int i = 0; i < 10; i++)
            {
                buf[i] = (byte)(0x92 | (data & 1) | ((data & 2) << 2) | ((data & 4) << 4));
                data >>= 3;
            }

            buf[10] = (byte)(0xf2 | (data & 1) | ((data & 2) << 2));

            port.Write(buf, 0, 11);
        }

        private bool GetAck(int retries)
        {
            for (int i = 0; i < retries; i++)
            {
                port.Write(new byte[] { 0xF9 }, 0, 1);

                if (port.BytesToRead > 0)
                    return true;

                Thread.Sleep(10);
            }


            return false;
        }

        #region IDisposable implementation

        public void Dispose()
        {
            lfsrBuffer = null;
            receiveBuffer = null;

            if (port != null)
            {
                if (port.IsOpen)
                    port.Close();

                port.Dispose();
            }
        }

        #endregion
    }

    public class CmdOption
    {
        private string flag;
        private string value;

        public string Flag => flag;

        public string Value => value;

        public CmdOption(string flag, string value)
        {
            this.flag = flag;
            this.value = value;
        }
    }

    public class Cmd
    {
        private List<CmdOption> options = new List<CmdOption>();

        public CmdOption this[string flag]
        {
            get
            {
                string f1 = string.IsNullOrEmpty(flag) ? null : flag.ToLower().Trim(new char[] { '-' });

                for (int i = 0; i < options.Count; i++)
                {
                    string f2 = string.IsNullOrEmpty(options[i].Flag) ? null : options[i].Flag.ToLower().Trim(new char[] { '-' });
                    if (f2 == f1)
                        return options[i];
                }

                return null;
            }
        }

        public static Cmd Parse(string[] args)
        {
            Cmd cmd = new Cmd();

            if (args == null)
                return cmd;

            for (int i = 0; i < args.Length; i++)
            {
                bool isFlag = args[i].StartsWith("-");
                bool hasParam = (i + 1) < args.Length ? !args[i + 1].StartsWith("-") : false;

                string parameter = hasParam ? args[i + 1] : null;
                string arg = args[i].TrimStart(new char[] { '-' });

                if (isFlag)
                {
                    char[] argChars = arg.ToCharArray();

                    if (args[i].StartsWith("--"))
                        cmd.Push(arg, parameter);
                    else
                    {
                        if (arg.Length == 1)
                            cmd.Push(arg, parameter);
                        else
                            for (int c = 0; c < argChars.Length; c++)
                                cmd.Push(argChars[c].ToString(), null);
                    }

                    if (argChars.Length == 1 && hasParam)
                        i++;
                }
                else
                    cmd.Push(null, arg);
            }

            return cmd;
        }

        public void Push(string flag, string value)
        {
            options.Add(new CmdOption(flag, value));
        }
    }
}