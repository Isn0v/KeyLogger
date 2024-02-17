import os
from time import sleep
import log
from os import path, remove, listdir

DELAY = 60
MESSAGE = 'GUI'
DIRECTORY = '../EmailsToSend'
if __name__ == "__main__":
    while True:
        files = [DIRECTORY+'/'+file for file in listdir(DIRECTORY) if file.endswith('.txt')]
        log.send(MESSAGE, files)
        for file in files:
            if path.exists(file):
                remove(file)
        sleep(DELAY)
