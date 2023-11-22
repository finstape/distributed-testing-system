from __future__ import absolute_import, unicode_literals
import os
from celery import Celery
# Устанавливаем переменную окружения, чтобы указать Django на файл настроек
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'dts.settings')

# Создаем экземпляр Celery
app = Celery('dts')
broker_connection_retry_on_startup = True 
BROKER_CONNECTION_TIMEOUT = 10
broker_transport_options = {'credentials_transport': 'AMQPLAIN'}

# Загружаем конфигурацию из файла Django settings.py
app.config_from_object('django.conf:settings', namespace='CELERY')

# Автоматически обнаруживаем и регистрируем задачи из файлов tasks.py в приложениях Django
app.autodiscover_tasks()
