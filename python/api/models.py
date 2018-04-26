from django.db import models

# Create your models here.
class Bucketlist(models.Model):
    """This class represents the bucketlist model."""
    name = models.CharField(max_length=255, blank=False, unique=True)
    date_created = models.DateTimeField(auto_now_add=True)
    date_modified = models.DateTimeField(auto_now=True)

    def __str__(self):
        """Return a human readable representation of the model instance."""
        return "{}".format(self.name)


class Indeed(models.Model):
    jobtitle = models.CharField(max_length=255, blank=True, null=True)
    company = models.CharField(max_length=255, blank=True, null=True)
    city = models.CharField(max_length=50, blank=True, null=True)
    state = models.CharField(max_length=25, blank=True, null=True)
    snippet = models.CharField(max_length=500, blank=True, null=True)
    url = models.CharField(max_length=500, blank=True, null=True)
    longitutde = models.FloatField(blank=True, null=True)
    latitutde = models.FloatField(blank=True, null=True)
    def __str__(self):
        template = '{0.jobtitle} {0.company} {0.city} {0.state} {0.snippet} {0.url}'
        return template.format(self)
    class Meta:
        managed = False
        db_table = 'indeed'
