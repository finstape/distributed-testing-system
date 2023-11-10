from __future__ import absolute_import
from typing import Any
from django.db import models
from django.db.models.query import QuerySet
from django.shortcuts import render, get_object_or_404
from django.http import HttpResponse, Http404, HttpResponseRedirect
from django.template import loader
from django.urls import reverse
from django.views import generic
from django.utils import timezone
from django.http import JsonResponse

from django.db.models import F, Count

from .models import Task, CTest, CodeSubmit

from .tasks import process_code_cpp


def submit(request, t_id):
    if request.method == 'POST' and request.headers.get('x-requested-with') == 'XMLHttpRequest':
        t_data = request.POST.get('task_data')
        print(t_data)
        print(type(t_data))

        # сохранение данных в бд
        # task = Task.objects.get(pk=t_id)
        # CodeSubmit.objects.create(task=task, code=t_data, submit_result = 0)

        result = process_code_cpp.delay(t_data)
        
        print(result.state, result.info, type(result))
        return JsonResponse({'message': f'Результат: {result}'})
    return JsonResponse({'message': 'Неверный запрос'}, status=400)

class IndexView(generic.ListView):
    template_name = "tasks/index.html"
    context_object_name = "lastest_tasks_list"
    def get_queryset(self) -> QuerySet[Any]:
        return Task.objects.order_by("-id")[:5]

def task(request, t_id):
    t = get_object_or_404(Task, pk=t_id)
    return render(request, "tasks/detail.html", {"task": t},)

