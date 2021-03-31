#### Build images
docker build -f Dockerfile.dev -t stockdb_image_dev .
docker build -f Dockerfile.prod -t stockdb_image_prod .

#### Run dev image
docker run -it --name stockdb_container_dev -v /Users/annotelldaniel/docker_mount:/root/mnt -p 7352:7352 -p 6934:6934 stockdb_image_dev fish

#### Run prod image
docker run -it --name stockdb_container_prod -v /Users/annotelldaniel/docker_mount:/root/mnt -p 7352:7352 -p 6934:6934 stockdb_image_prod fish

#### Stop container
docker stop [container hash]

#### Delete container
docker container rm [container hash]

#### Delete image
docker rmi [container hash]

#### Start new fish shell in running docker
docker exec -it [container hash] fish

#### List images
docker images

#### List containers (stopped and running)
docker ps -a

# Other stuff
service ssh start
ssh root@localhost -p 7352

Att göra i nybyggd docker:
 - Ändra porten för ssh i /etc/ssh/sshd_config
 - Lägg till client-pub-key (Mac-klienten) i /root/.ssh/authorized_keys
 - Starta ssh
