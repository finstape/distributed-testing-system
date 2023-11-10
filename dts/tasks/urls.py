from django.urls import path
from . import views


app_name='tasks'
urlpatterns = [
    path("", views.IndexView.as_view(), name="index"),
    path("<int:t_id>/", views.task, name='task_details'),
    path("<int:t_id>/submit", views.submit, name='submit'),
]
