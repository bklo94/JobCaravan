from django.db import models
from django.db.models import F
import unittest
# Create your models here.

class Indeed(models.Model):
    jobtitle = models.CharField(max_length=255, blank=True, null=True)
    company = models.CharField(max_length=255, blank=True, null=True)
    city = models.CharField(max_length=50, blank=True, null=True)
    state = models.CharField(max_length=25, blank=True, null=True)
    snippet = models.CharField(max_length=500, blank=True, null=True)
    url = models.CharField(max_length=500, blank=True, null=True)
    longitude = models.FloatField(blank=True, null=True)
    latitude = models.FloatField(blank=True, null=True)
    reldate = models.CharField(max_length=255, blank=True, null=True)
    postdate = models.CharField(max_length=255, blank=True, null=True)
    def updateDB(self):
        obj = Indeed.objects.first()
        obj.refresh_from_db()
    def __str__(self):
        template = '{0.jobtitle} {0.company} {0.city} {0.state} {0.snippet} {0.url} {0.reldate} {0.postdate}'
        return template.format(self)
    class Meta:
        managed = False
        db_table = 'indeed'
