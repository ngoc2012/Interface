from django.urls import path

from . import views

# https://docs.djangoproject.com/en/2.1/howto/static-files/#serving-files-uploaded-by-a-user-during-development

from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    path('<str:userName>/<path:fileName>', views.getFile),
]
# ] + static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
