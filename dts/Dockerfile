# Use an official Python runtime as a parent image
FROM python:3.10-slim

# Expose the necessary port
EXPOSE 8083

# Keeps Python from generating .pyc files in the container
ENV PYTHONDONTWRITEBYTECODE=1

# Turns off buffering for easier container logging
ENV PYTHONUNBUFFERED=1

# Install system dependencies for psycopg2, RabbitMQ, and Redis
RUN apt-get update && \
    apt-get install -y libpq-dev gcc python3-dev && \
    apt-get install -y rabbitmq-server && \
    apt-get install -y redis-server && \
    rm -rf /var/lib/apt/lists/*

# Install pip requirements
COPY requirements.txt .
RUN python -m pip install --no-cache-dir -r requirements.txt

# Set the working directory in the container
WORKDIR /app

# Copy the application files into the container
COPY . /app

# Creates a non-root user with an explicit UID and adds permission to access the /app folder
RUN adduser -u 5678 --disabled-password --gecos "" appuser && chown -R appuser /app

# Switch to the non-root user
USER appuser
ENV DJANGO_ALLOWED_HOSTS="your-cluster-domain.com, your-ip-address"

# During debugging, this entry point will be overridden.
# For more information, please refer to https://aka.ms/vscode-docker-python-debug
CMD ["daphne", "dts.asgi:application", "-b", "0.0.0.0", "-p", "8083"]
