sudo apt update && sudo apt upgrade
sudo apt install python3.11 python3.11-dev python3.11-pip python3.11-venv
pip3.11 install -r worker_requirements.txt
cd dts
python3.11 -m celery -A dts worker
