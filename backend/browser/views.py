from os import listdir, walk, stat

from django.http import JsonResponse, HttpResponse

# from "../admin.admin" import getFullName, checkFolder, checkFile

from admin.admin import usersFolder

def getData(request, folderName):
    # import json
    # data = json.loads(request.body.decode('utf-8'))

    # user = data['user']
    # checkUser(user)
    username = folderName.split("/")[0]
    print(username)
    folderName = ""
    print("folderName:", folderName)
    import os.path, time
    def getList(userFolder, folderName):
        fileFoldersList = []
        for root, localFolders, localFiles in walk(userFolder + '/' + folderName):
            # Le cas des dossiers vides:
            for f in localFolders:

                if len(listdir(root + '/' + f)) == 0:
                    fileFoldersList.append({
                        'key': (root.replace(userFolder,"") + '/' + f).replace('\\', '/'),
                        'modified': 0,
                        'size': 0
                    })
            # Les fichiers
            for f in localFiles:
                # print(time.gmtime(os.path.getmtime(root + '/' + f)))
                fileFoldersList.append({
                    'key': (root.replace(userFolder,"") + '/' + f).replace('\\', '/'),
                    # 'modified': stat(root + '/' + f).st_mtime*1000,
                    'created': time.ctime(os.path.getctime(root + '/' + f)),
                    'modified': time.strftime(r"%d/%m/%Y %H:%M:%S", time.gmtime(os.path.getmtime(root + '/' + f))),
                    'size': stat(root + '/' + f).st_size
                })
        
        # print(fileFoldersList)
        return fileFoldersList
    '''
    moment.js
    X	1410715640.579	Unix timestamp
    x	1410715640579	Unix ms timestamp
    It is the number of seconds that have elapsed since the Unix epoch, that is the time 00:00:00 UTC on 1 January 1970, minus leap seconds.
    '''

    userFolder = usersFolder + '/' + username # + '/' + folderName
    if os.path.isdir(userFolder):
        return JsonResponse({"files": getList(userFolder, folderName)})
    else:
        return HttpResponse("Error: Folder " + username + '/' + folderName + " does not exist")