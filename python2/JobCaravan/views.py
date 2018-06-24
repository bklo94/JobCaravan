# Copyright 2015 Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from django.shortcuts import render
from django.http import HttpResponse
from django.http import JsonResponse
from django.template import loader
import pprint
import re

# Create your views here.

from rest_framework import generics
from .models import Indeed


def getWords(text):
    return re.compile('\w+').findall(text)

def convertMonth(text):
    return{
        'Jan': '01',
        'Feb': '02',
        'Mar': '03',
        'Apr': '04',
        'May': '05',
        'Jun': '06',
        'Jul': '07',
        'Aug': '08',
        'Sep': '09',
        'Oct': '10',
        'Nov': '11',
        'Dec': '12',
    }[text]

def table(request):
    update = Indeed()
    update.updateDB()
    #filters the bolded snippets and formats dates
    for target in Indeed.objects.all():
        #Cleanse the snippet
        temp = target.snippet
        temp = temp.replace('<b>','').replace('</b>','')
        target.snippet = temp
        #Format the date
        temp = target.postdate
        tempArr = getWords(temp)
        if (len(tempArr) == 3):
            target.save()
            break
        else:
            tempArr[2] = convertMonth(tempArr[2])
            dateFormat = tempArr[2] + "-" + tempArr[1] +"-"+ tempArr[3]
            target.postdate= dateFormat
            #save the data
            target.save()

    jobList = list(Indeed.objects.all())
    template = loader.get_template('indeed/table.html')
    context = {
        'jobList': jobList,
    }
    length = len(jobList)
    print ("Number of Jobs -", length)
    return HttpResponse(template.render(context, request))

def leaflet(request):
    update = Indeed()
    update.updateDB()
    jobList = list(Indeed.objects.all())
    template = loader.get_template('indeed/leaflet.html')
    context = {
        'jobList': jobList,
    }
    length = len(jobList)
    print ("Number of Jobs -", length)
    return HttpResponse(template.render(context, request))


def index(request):
    return render(request,'indeed/index.html')

def main(request):
    update = Indeed()
    update.updateDB()
    jobList = list(Indeed.objects.all())
    template = loader.get_template('indeed/main.html')
    context = {
        'jobList': jobList,
    }
    length = len(jobList)
    print ("Number of Jobs -", length)
    return HttpResponse(template.render(context, request))
