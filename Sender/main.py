from time import sleep
import log as log
from os import path, remove, listdir

DELAY = 60
MESSAGE = 'GUI'
DIRECTORY = '../EmailsToSend'
if __name__ == "__main__":
    while True:
        files = [file for file in listdir(DIRECTORY) if file.endswith('.txt')]
        log.send(MESSAGE, files)
        for file in files:
            if path.exists(f'{DIRECTORY}/{file}'):
                remove(f'{DIRECTORY}/{file}')
        sleep(DELAY)
