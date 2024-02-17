import smtplib, ssl
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email import encoders
from email.mime.base import MIMEBase
from jproperties import Properties


p = Properties()
with open('mail.properties', 'rb') as config_file:
    p.load(config_file, 'utf-8')
    
sender_email = p.get("sender_email").data
receiver_email = p.get("receiver_email").data
password = p.get("password").data
    
subject = "Log Dump of recent session"
body = ""

message = MIMEMultipart()
message["From"] = sender_email
message["To"] = receiver_email
message["Subject"] = subject
message["Bcc"] = receiver_email


message.attach(MIMEText(body, "plain"))

filename = p.get("filename").data
with open(filename, "rb") as attachment:
    part = MIMEBase("application", "octet-stream")
    part.set_payload(attachment.read())
    
encoders.encode_base64(part)

part.add_header(
    "Content-Disposition",
    f"attachment; filename= {filename}",
)

message.attach(part)
text = message.as_string()

context = ssl.create_default_context()
with smtplib.SMTP_SSL("smtp.gmail.com", 465, context=context) as server:
    server.login(sender_email, password)
    server.sendmail(sender_email, receiver_email, text)
