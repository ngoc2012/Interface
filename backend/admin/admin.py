from django.http import JsonResponse
from django.http import HttpResponse
from django.utils.encoding import uri_to_iri

from os import listdir, walk, stat, getcwd, path
folder = path.dirname(getcwd())
folder = folder.split('\\')
folder = '\\'.join(folder[:-1])

print("**************", folder)

# ================== Ubuntu ===================
# folder = "/media/sf_Gcpm"

# defaults = importJson(folder + "/backend/defaults.json")
# usersFolder = defaults['usersFolder']
usersFolder = folder + "/users"


def checkFolder(user):
    if user not in listdir(usersFolder):
        return HttpResponse("Le dossier de l'utilisateur '{}' n'existe pas".format(user), status=400)

def checkFile(fileName, fullName):
    import os.path
    if not os.path.isfile(fullName):
        # print("error", os.path.isfile(usersFolder + '/' + fileName))
        return HttpResponse("Le fichier '{}' n'existe pas".format(fileName), status=400)

def getFullName(user, fileName):
    filenameTxt = fileName.split('.')
    if len(filenameTxt) == 1:
        # Fichier pas d'extension => json
        fullName =  usersFolder + '/' + user + '/' + fileName + ".json"
        extension = "json"
    else:
        extension = filenameTxt[-1].lower()
    return fullName, extension

def getExtension(fileName):
    filenameTxt = fileName.split('.')
    if len(filenameTxt) == 1:
        return ""
    else:
        return filenameTxt[-1].lower()

