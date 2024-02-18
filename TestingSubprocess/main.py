import subprocess
import signal
import time
import os
ar=[]
# Start the process
process = subprocess.Popen(["./TestSubprocess"])
while True:
    process.send_signal(signal.SIGSTOP)
    file_list = [f for f in os.listdir("Dir") if f.endswith(".txt")]
    for file in file_list:
        with open(os.path.join("Dir", file), 'r') as f:
            ar.append(f.read())
        os.remove(os.path.join("Dir", file))
    ar.clear()
    process.send_signal(signal.SIGCONT)# Resume the process
    time.sleep(6)