from admin.admin import folder, usersFolder, checkFolder, checkFile, getFullName, getExtension

import sys
sys.path.insert(0, folder + "/codes")

from importJson import importJson

def getCppOutput(cmd):
    from subprocess import Popen, PIPE
    outputTxt = []
    # print(cmd)
    # Not show the cout output string
    with Popen(cmd, stdout=PIPE, bufsize=1, universal_newlines=True) as p:
        for line in p.stdout:
            outputTxt.append(line)
    
    import json
    # print("\n".join(outputTxt))
    output = json.loads("\n".join(outputTxt))
    # print(output)
    return output

    # proc = Popen(cmd)
    # proc.communicate()
    # return {}

def BASecRecDimELU(inputData):
    
    f = folder + "/Interface/backend/editor/"
    params = ['b', 'h', 'enrobage', 'MEd', 'fck', 'fyk', 'Eb', 'Ea']
    cmd = [f + 'BASecRecDimELU.exe'] + ["on"] + ["BASecRecDimELU"] + [','.join(params)] + [str(inputData[i]) for i in params]
    # cmd = [f + 'BASecRecDimELU.exe'] + [str(inputData[i]) for i in params]

    return getCppOutput(cmd)
    # smt = getCppOutput(cmd)
    # return {}

def getOutput(inputData):
    if inputData['type'] == "BASecRecDimELU":
        # ! Wait until the tex file is created completely
        # from threading import Thread
        # thread = Thread(target=BASecRecDimELU(inputData))
        # thread.start()
        # return {}
        return BASecRecDimELU(inputData)
    return {}

def getJsonData(fileJson):
    data = {
        'type': 'json',
        'termes': importJson(folder + '/Interface/backend/editor/Tex/' + fileJson['type'] + ".termes.json"),
        'defaults': importJson(folder + '/Interface/backend/editor/Tex/' + fileJson['type'] + ".defaults.json"),
        'tex': getTex(fileJson),
        'input': fileJson, 
        # 'output': getOutput(fileJson)
        'output': {}
    }
    return data

def getTex(fileJson):
    # C:\Users\mnnguyen\Documents\Gcpm\Interface\backend\editor\Tex
    texFile = folder + '/Interface/backend/editor/Tex/' + fileJson['type'] + '.tex'
    txt = ""
    # Get text between begin{document} and end{document}
    with open(texFile, encoding='utf-8') as f:
        inDocument = False
        for line in f:
            if line[:14] == r"\end{document}":
                inDocument = False
            if inDocument:
                txt += line
            if line[:16] == r"\begin{document}":
                inDocument = True
    return txt

def checkJsonFile(fullName):
    try:
        fileJson = importJson(fullName)
    except ValueError:
        return {'type': 'error', 'status': 'Le fichier est erroné'}
    if 'type' not in fileJson.keys():
        return {'type': 'error', 'status': "Le type du fichier n'est pas précisé"}
    return fileJson

from django.http import JsonResponse, HttpResponse
from admin.admin import usersFolder

def getFile(request, userName, fileName = ""):
    # global user, fileName, fullName
    
    # return JsonResponse({"smt": "smt"})

    # return HttpResponse("smt")

    # import json
    # data = json.loads(request.body.decode('utf-8'))
    # user = data['user']
    # fileName = data['fileName']

    # fullName, extension = getFullName(userName, fileName)
    # checkFolder(user)
    # checkFile(fileName, fullName)
    
    fullFileName = usersFolder + '/' + userName + '/' + fileName
    print(fullFileName)
    
    if getExtension(fullFileName) == 'json':
        
        fileJson = checkJsonFile(fullFileName)
        if fileJson['type'] == 'error':
            return JsonResponse(fileJson)

        if fileJson['type'] == 'projetParoi':
            next
            print(userName)
            print(fileName)
            print(fileName.split('/')[:-1])
            # f = folder + "/Interface/backend/editor/"
            # params = ['b', 'h', 'enrobage', 'MEd', 'fck', 'fyk', 'Eb', 'Ea']
            # cmd = [f + 'BASecRecDimELU.exe'] + ["on"] + ["BASecRecDimELU"] + [','.join(params)] + [str(inputData[i]) for i in params]

            # from subprocess import Popen, PIPE
            # outputTxt = []
            # # print(cmd)
            # # Not show the cout output string
            # with Popen(cmd, stdout=PIPE, bufsize=1, universal_newlines=True) as p:
            #     for line in p.stdout:
            #         outputTxt.append(line)
            
            # import json
            # # print("\n".join(outputTxt))
            # output = json.loads("\n".join(outputTxt))
            fileJson = usersFolder + '/' + userName + '/' + fileName;
            return JsonResponse(fileJson)
            
        if fileJson['type'] == 'graphRidoBA':
            fileJson["graph"] = importJson(folder + '/Interface/backend/editor/' + fileJson['type'] + ".json")
            return JsonResponse(fileJson)

        # print(JsonResponse(getJsonData(fileJson)))
        return JsonResponse(getJsonData(fileJson))

    return JsonResponse({'type': 'test', 'tex': "I'm good"})