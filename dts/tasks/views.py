from __future__ import absolute_import
from typing import Any
from django.db.models.query import QuerySet
from django.shortcuts import render, get_object_or_404
from django.views import generic
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt

from .models import Task

import tasks
from celery.result import AsyncResult


def get_result(request, task_id):
    try:
        # Получение результата по идентификатору задачи
        result = AsyncResult(task_id).get()
        return JsonResponse({'message': f'Результат: {result}'})
    except Exception as e:
        return JsonResponse({'message': f'Произошла ошибка: {str(e)}'}, status=500)


@csrf_exempt
def submit(request, task_id):
    if request.method == 'POST' and request.headers.get('x-requested-with') == 'XMLHttpRequest':
        t_data = request.POST.get('task_data')
        print(t_data)
        print(type(t_data))
        print(task_id)

        # сохранение данных в бд
        # task = Task.objects.get(pk=t_id)
        # CodeSubmit.objects.create(task=task, code=t_data, submit_result=0)
        process = tasks.process_code_cpp.delay(t_data)
        print(process.id)

        # Возвращаем идентификатор задачи
        return JsonResponse({'process_id': process.id})
    return JsonResponse({'message': 'Неверный запрос'}, status=400)


class IndexView(generic.ListView):
    template_name = "tasks/index.html"
    context_object_name = "lastest_tasks_list"

    def get_queryset(self) -> QuerySet[Any]:
        return Task.objects.order_by("-id")[:5]


def task(request, task_id):
    t = get_object_or_404(Task, pk=task_id)
    return render(request, "tasks/detail.html", {"task": t}, )
