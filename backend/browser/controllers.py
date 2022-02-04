import os
from django.http import JsonResponse, HttpResponse

from admin.admin import usersFolder

def getLocalFolder(fullFileName):
    localFolder = fullFileName.split('/')
    localFolder = '/'.join(localFolder[:(len(localFolder)-1)]) + '/'
    return localFolder

def getFileExtension(fileName):
    if fileName == fileName.split('.')[-1]:
        return ""
    else:
        return fileName.split('.')[-1]

def getFileNameWithoutDuplicated(localFolder, fileName):
    
    if not os.path.exists(localFolder + fileName):
        return fileName

    extension = getFileExtension(fileName)
    fileNameWithoutExtension = '.'.join(fileName.split('.')[:-1])
    i = 1
    while os.path.isfile(localFolder + fileNameWithoutExtension + str(i) + '.' + extension):
        i += 1

    newFileName = fileNameWithoutExtension + str(i) + '.' + extension

def getFileNameWithoutFolder(fileName):
    return fileName.split('/')[-1]

def files(request, operation, username, fileName):
    import json
    data = json.loads(request.body.decode('utf-8'))
    
    # if not checkUser(user)

    status = "Success"
    
    userFolder = usersFolder + '/' + username + '/'
    fullFileName = userFolder + fileName
    fileNameWithoutFolder = getFileNameWithoutFolder(fileName)

    if not os.path.isfile(fullFileName):
        return HttpResponse("Error: File " + fileName + " does not exist")
        
    import shutil

    if operation == "duplicate":
        
        localFolder = getLocalFolder(fileName)
        localUserFolder = userFolder + localFolder

        newFileName = getFileNameWithoutDuplicated(localUserFolder, fileNameWithoutFolder)
        shutil.copyfile(fullFileName, localUserFolder + newFileName)

        return JsonResponse({'status': "Fichier {} a été créé".format(newFileName), 'newFileName': newFileName})
    
    if operation == "delete":
        if not os.path.isfile(fullFileName):
            return HttpResponse("Error: File " + fileName + " does not exist")
        os.remove(fullFileName)
        return JsonResponse({'status': "Fichier {} a été supprimé".format(fileName)})
    
    if operation == "rename":
        if "newname" not in data.keys():
            HttpResponse("Error: New file name does not specified")

        newFileName = data['newname']
        
        localFolder = getLocalFolder(fileName)
        localUserFolder = userFolder + localFolder

        newFullFileName = localUserFolder + newFileName

        if os.path.isfile(newFullFileName):
            return HttpResponse("Error: Cannot rename, file name " + localFolder + '/' + newFileName + " existed")

        os.rename(fullFileName, newFullFileName)
        return JsonResponse({'status': "Fichier {} a été renommé en {}".format(fileName, localFolder + '/' + newFileName)})
    
    if operation == "moveto":
        if "newfolder" not in data.keys():
            HttpResponse("Error: New folder name does not specified")

        newFolder = data['newfolder']
        newUserFolder = userFolder + newFolder

        if not os.path.isdir(newUserFolder):
            return HttpResponse("Error: Folder " + newFolder + " does not exist")

        newFullFileName = newUserFolder + '/' + fileNameWithoutFolder
        if os.path.isfile(newFullFileName):
            return HttpResponse("Error: Cannot move, file name " + newFolder + '/' + fileNameWithoutFolder + " exist")

        shutil.copyfile(userFolder + fileName, newFullFileName)
        os.remove(fullFileName)

        return JsonResponse({'status': "Fichier {} a été déplacé vers {}".format(fileName, newFolder + '/' + fileNameWithoutFolder)})
    
    if operation == "copyto":
        newFolder = data['newfolder']
        newUserFolder = userFolder + newFolder
        if not os.path.isdir(newUserFolder):
            return HttpResponse("Error: Folder " + newFolder + " does not exist")

        newFullFileName = newUserFolder + '/' + fileNameWithoutFolder
        if os.path.isfile(newFullFileName):
            return HttpResponse("Error: Cannot move, file name " + newFolder + '/' + fileNameWithoutFolder + " exist")

        shutil.copyfile(userFolder + fileName, newFullFileName)
        return JsonResponse({'status': "Fichier {} a été créé".format(newFolder + '/' + fileNameWithoutFolder)})

    # if operation == "newfolder": ???
    return HttpResponse("Success", status=200)