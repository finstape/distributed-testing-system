pip install -r worker_requirements.txt
cd dts
python -m celery -A dts worker
