from typing import Any
from django.db.models.query import QuerySet
from django.shortcuts import render, get_object_or_404
from django.views import generic
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from asgiref.sync import sync_to_async

from .models import Task

from tasks.tasks import process_code_cpp
from celery.result import AsyncResult

@sync_to_async
def get_result_async(task_id):
    try:
        # Получение результата по идентификатору задачи
        result = AsyncResult(task_id).get()
        return {'message': f'Результат: {result}'}
    except Exception as e:
        return {'message': f'Произошла ошибка: {str(e)}'}, 500

@csrf_exempt
@sync_to_async
def submit_async(request, task_id):
    if request.method == 'POST' and request.headers.get('x-requested-with') == 'XMLHttpRequest':
        t_data = request.POST.get('task_data')
        print(t_data)
        print(type(t_data))
        print(task_id)

        # сохранение данных в бд
        # task = Task.objects.get(pk=t_id)
        # CodeSubmit.objects.create(task=task, code=t_data, submit_result=0)
        process = process_code_cpp.delay(t_data, task_id)
        print(process.id)

        # Возвращаем идентификатор задачи
        return {'process_id': process.id}
    return {'message': 'Неверный запрос'}, 400

class IndexView(generic.ListView):
    template_name = "tasks/index.html"
    context_object_name = "lastest_tasks_list"

    async def get_queryset(self) -> QuerySet[Any]:
        return await sync_to_async(Task.objects.order_by("-id")[:5])

async def base(request):
    return await sync_to_async(lambda: render(request, "tasks/main.html"))

async def task(request, task_id):
    t = await sync_to_async(lambda: get_object_or_404(Task, pk=task_id))
    return await sync_to_async(lambda: render(request, "tasks/detail.html", {"task": t}))

async def get_result(request, task_id):
    result = await get_result_async(task_id)
    return JsonResponse(result)

async def submit(request, task_id):
    result = await submit_async(request, task_id)
    return JsonResponse(result)
