from django.urls import path
from . import views

app_name = 'tasks'
urlpatterns = [
    path("", views.IndexView.as_view(), name="index"),
    path("<int:task_id>/", views.task, name='task_details'),
    path('get_result/<str:task_id>/', views.get_result, name='get_result'),
    path("<int:task_id>/submit", views.submit, name='submit'),
]
