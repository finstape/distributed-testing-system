from django.contrib import admin
from .models import Task, CTest


class CTestInLine(admin.TabularInline):
    model = CTest
    extra = 3


class TaskAdmin(admin.ModelAdmin):
    fieldsets = [
        (None, {"fields": ["task_name", "description"]}),
    ]
    inlines = [CTestInLine]
    list_display = ["task_name", "description"]
    search_fields = ["task_name"]


admin.site.register(Task, TaskAdmin)
admin.site.register(CTest)
# Register your models here.
