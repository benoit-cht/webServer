docker rm -f webserv-test 2>/dev/null

docker run --name webserv-test \
  -p 8080:8080 \
  -v "$(pwd)/.config/test_nginx.conf":/etc/nginx/nginx.conf:ro \
  -v "$(pwd)/www_folder/html":/var/www/html:ro \
  -d nginx
