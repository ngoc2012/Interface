from django.urls import path

from . import views

# https://docs.djangoproject.com/en/2.1/howto/static-files/#serving-files-uploaded-by-a-user-during-development

from django.conf import settings
from django.conf.urls.static import static

# from views import getData

urlpatterns = [
    # path('<str:username>', views.getData),
    # path('<str:username>/<path:folderName>', views.getData),
    # path('files/<str:operation>/<str:username>/<path:fileName>', views.getData),
    path('<path:folderName>', views.getData),
] + static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
