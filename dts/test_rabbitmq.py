import pika

credentials = pika.PlainCredentials(username='dj-dts', password='zumfy6-zYkteg-mivvav')
parameters = pika.ConnectionParameters(host='localhost', credentials=credentials, port='5672')
connection = pika.BlockingConnection(parameters)

# Если выполнение дошло до этой строки, соединение успешно установлено.
print("Соединение с RabbitMQ установлено.")
connection.close()