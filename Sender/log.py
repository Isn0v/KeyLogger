import os
import pickle
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from base64 import urlsafe_b64encode
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from mimetypes import guess_type as guess_mime_type


def gmail_authenticate():
    creds = None
    if os.path.exists("token.pickle"):
        with open("token.pickle", "rb") as token:
            creds = pickle.load(token)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file('Sender/credentials.json', SCOPES)
            creds = flow.run_local_server(port=0)
        with open("token.pickle", "wb") as token:
            pickle.dump(creds, token)
    return build('gmail', 'v1', credentials=creds)


SCOPES = ['https://mail.google.com/']
DESTINATION = 'osipovichivan99@gmail.com'
SERVICE = gmail_authenticate()
OBJ = 'Test guev'
SOURCE = 'osipovichivan99@gmail.com'


# Adds the attachment with the given filename to the given message
def add_attachment(message, filename):
    content_type, encoding = guess_mime_type(filename)
    if content_type is None or encoding is not None:
        content_type = 'application/octet-stream'
    main_type, sub_type = content_type.split('/', 1)  # main_type=text
    fp = open(filename, 'rb')
    msg = MIMEText(fp.read().decode(), _subtype=sub_type)
    fp.close()
    filename = os.path.basename(filename)
    msg.add_header('Content-Disposition', 'attachment', filename=filename)
    message.attach(msg)


def build_message(source, destination, obj, body, attachments: list):  # list of txt
    message = MIMEMultipart()
    message['to'] = destination
    message['from'] = source
    message['subject'] = obj
    message.attach(MIMEText(body))
    for filename in attachments:
        add_attachment(message, filename)
    return {'raw': urlsafe_b64encode(message.as_bytes()).decode()}


def send_message(service, source, destination, obj, body, attachments: list):
    return service.users().messages().send(
        userId="me",
        body=build_message(source, destination, obj, body, attachments)
    ).execute()


def send(body, attachments: list):
    send_message(SERVICE, SOURCE, DESTINATION, OBJ, body, attachments)
