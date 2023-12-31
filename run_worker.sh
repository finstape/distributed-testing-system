sudo apt update && sudo apt upgrade
sudo apt install python3.11 python3.11-dev python3.11-venv
python3.11 -m pip install -r worker_requirements.txt
cd dts
python3.11 -m celery -A dts worker --loglevel=info --detach --logfile=/tmp/dts_worker.log -n worker@%h
