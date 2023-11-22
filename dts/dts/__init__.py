from __future__ import absolute_import, unicode_literals

# Это обязательная строка для обеспечения работы Celery с Django
from .celery import app as celery_app
__all__ = ['celery_app']