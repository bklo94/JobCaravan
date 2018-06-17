FROM python:3
ENV PYTHONUNBUFFERED 1
RUN mkdir /frontend
WORKDIR /frontend
ADD requirements.txt /frontend/
RUN pip install -r requirements.txt
ADD . /frontend/

EXPOSE 10801
