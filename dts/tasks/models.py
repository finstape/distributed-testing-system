from django.db import models


# Create your models here.

class Task(models.Model):
    task_name = models.CharField(max_length=100)
    description = models.TextField()
    input_data = models.CharField(max_length=1000)
    output_data = models.CharField(max_length=1000)

    def __str__(self):
        return self.task_name


class CTest(models.Model):
    task = models.ForeignKey(Task, on_delete=models.CASCADE)
    test = models.TextField()
    test_result = models.TextField()

    def __str__(self):
        return self.test


class CodeSubmit(models.Model):
    task = models.ForeignKey(Task, on_delete=models.CASCADE)
    code = models.TextField()
    submit_result = models.BooleanField(default=False)
