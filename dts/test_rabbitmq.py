import pika

credentials = pika.PlainCredentials(username='dj-dts', password='zumfy6-zYkteg-mivvav')
parameters = pika.ConnectionParameters(host='localhost', credentials=credentials, port='5672')
connection = pika.BlockingConnection(parameters)

print("соединение с rabbitmq установлено")
connection.close()
