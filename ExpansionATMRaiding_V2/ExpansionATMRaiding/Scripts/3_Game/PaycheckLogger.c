class PaycheckLogger
{
    protected string logFilePath;

    void PaycheckLogger()
    {
        CreateLogFile();
    }

    void CreateLogFile()
    {
        // Use config path for logs
        string logFolder = "$profile:"
            + "\\ExpansionMod\\Paycheck\\Logs";
        if (!FileExist(logFolder))
        {
            MakeDirectory(logFolder);
        }
        logFilePath = logFolder + "\\PayCheckLog_" + GetTimestamp() + ".txt";
        FileHandle file = OpenFile(logFilePath, FileMode.WRITE);
        if (file != 0)
        {
            FPrintln(file, "[Paycheck] Log file created: " + GetTimestamp());
            CloseFile(file);
        }
    }

    void LogPaycheck(string logMessage)
    {
        // Always ensure log file exists (in case config path changes)
        if (logFilePath == "" || !FileExist(logFilePath))
            CreateLogFile();
        FileHandle file = OpenFile(logFilePath, FileMode.APPEND);
        if (file != 0)
        {
            FPrintln(file, "[" + GetTimeString() + "] " + logMessage);
            CloseFile(file);
        }
    }

    string GetTimestamp()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        return year.ToString() + month.ToStringLen(2) + day.ToStringLen(2) + "_" + hour.ToStringLen(2) + minute.ToStringLen(2) + second.ToStringLen(2);
    }

    string GetTimeString()
    {
        int hour, minute, second;
        GetHourMinuteSecond(hour, minute, second);
        return hour.ToStringLen(2) + ":" + minute.ToStringLen(2);
    }
};

ref PaycheckLogger m_PaycheckLogger;

static PaycheckLogger GetPaycheckLogger()
{
    if (!m_PaycheckLogger)
    {
        m_PaycheckLogger = new PaycheckLogger;
    }
    return m_PaycheckLogger;
}
