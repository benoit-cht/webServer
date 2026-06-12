# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_status_code.sh                                :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/11 14:34:50 by bchallat          #+#    #+#              #
#    Updated: 2026/06/12 12:55:51 by bchallat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED='\e[31m'
BLUE='\e[34m'
GREEN='\e[32m'
YELLOW='\e[1;33m'
DEF='\e[0m'

PORT=8080
HOST="localhost"

# ══════════════════════════════════════════════════════════════════════ #
#                                                                        #
# ══════════════════════════════════════════════════════════════════════ #

echo -e ${BLUE} ""
echo "╔════════════════════════════════════════════════════════════╗"
echo "║                                                            ║"
echo "║                TEST OF STATUS CODE HTTP                    ║"
echo "║                                                            ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo -e ${DEF}"\n"

check_status() {
  local target=$1
  local server=$2
  local methode=$3

  if [ "$target" = "$server" ]; then
    echo -e ${GREEN}"   methode: ${methode} | return of serveur: ${server} | target: ${target}" ${DEF}
    return 0
  else
    echo -e ${RED}"   methode: ${methode} | return of serveur: ${server} | target: ${target}" ${DEF}
    return 1
  fi
}

pause() {
  echo -e ${YELLOW}"\nAppuyer sur Entree pour continuer..." ${DEF}
  read
}

# ══════════════════════════════════════════════════════════════════════ #
#                                                                        #
# ══════════════════════════════════════════════════════════════════════ #
echo -e ${BLUE} ""
echo " -- CODE OF SUCCESSFUL 2XX --"
echo -e ${DEF}"\n"

echo -e ${YELLOW}"TEST code 200 OK: http://$HOST:$PORT/"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:$PORT/)
check_status "200" "$STATUS" "GET "
echo ""

echo -e ${YELLOW}"TEST code 200 OK: http://$HOST:$PORT/index.html"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:$PORT/index.html)
check_status "200" "$STATUS" "GET "
echo ""

# Test 403 Forbidden (Accès refusé sans en-tête)
echo -e ${YELLOW}"TEST 403 Forbidden: http://$HOST:8081"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:8081/)
check_status "403" "$STATUS" "GET /protected"
echo ""

# Test 301 Moved Permanently
echo -e ${YELLOW}"TEST 301 Redirect: http://$HOST:$PORT/old-url"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" --max-redirs 0 http://$HOST:$PORT/old-url)
check_status "301" "$STATUS" "GET /old-url"
echo ""

#echo -e ${YELLOW}"TEST 201 Created: POST http://$HOST:$PORT/upload/text.txt"${DEF}
#STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X POST -d '{"key":"value"}' http://$HOST:$PORT/upload/text.txt)
#check_status "201" "$STATUS" "POST /create"
#echo ""

echo -e ${YELLOW}"TEST code 204 NO CONTENT: http://$HOST:$PORT/upload/exemple.txt"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE http://$HOST:$PORT/upload/exemple.txt)
check_status "204" "$STATUS" "DELETE "
echo ""

echo -e ${YELLOW}"TEST code 403 NO CONTENT: http://$HOST:$PORT/upload/forriben-delete.txt"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE http://$HOST:$PORT/upload/forriben-delete.txt)
check_status "403" "$STATUS" "DELETE "
echo ""

# ══════════════════════════════════════════════════════════════════════ #
#                                                                        #
# ══════════════════════════════════════════════════════════════════════ #

# Test 200 OK (GET)
echo -e ${YELLOW}"TEST GET 200 OK: http://$HOST:$PORT/"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:$PORT/)
check_status "200" "$STATUS" "GET /"

# Test 404 Not Found
echo -e ${YELLOW}"TEST 404 Not Found: http://$HOST:$PORT/nonexistent"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:$PORT/nonexistent)
check_status "404" "$STATUS" "GET /nonexistent"

# Test 405 Method Not Allowed (POST sur une route GET-only)
echo -e ${YELLOW}"TEST 405 Method Not Allowed: POST http://$HOST:$PORT/"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X POST http://$HOST:$PORT/)
check_status "405" "$STATUS" "POST /"

# Test 400 Bad Request (Requête mal formée)
echo -e ${YELLOW}"TEST 400 Bad Request: http://$HOST:$PORT/?badparam"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" "http://$HOST:$PORT/?badparam")
check_status "400" "$STATUS" "GET /?badparam"

# Test 403 Forbidden (Accès refusé sans en-tête)
echo -e ${YELLOW}"TEST 403 Forbidden: http://$HOST:8081"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:8081/)
check_status "403" "$STATUS" "GET /protected"

# Test 200 OK avec en-tête personnalisé
echo -e ${YELLOW}"TEST 200 OK avec Header: http://$HOST:$PORT/api"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -H "Authorization: Bearer token123" http://$HOST:$PORT/api)
check_status "200" "$STATUS" "GET /api (avec Header)"

# Test 301 Moved Permanently
echo -e ${YELLOW}"TEST 301 Redirect: http://$HOST:$PORT/old-url"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" --max-redirs 0 http://$HOST:$PORT/old-url)
check_status "301" "$STATUS" "GET /old-url"

# Test 302 Found (Redirection temporaire)
echo -e ${YELLOW}"TEST 302 Redirect: http://$HOST:$PORT/temp-redirect"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" --max-redirs 0 http://$HOST:$PORT/temp-redirect)
check_status "302" "$STATUS" "GET /temp-redirect"

# Test 201 Created (POST)
echo -e ${YELLOW}"TEST 201 Created: POST http://$HOST:$PORT/create"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X POST -d '{"key":"value"}' http://$HOST:$PORT/create)
check_status "201" "$STATUS" "POST /create"

# Test 204 No Content (DELETE)
echo -e ${YELLOW}"TEST 204 No Content: DELETE http://$HOST:$PORT/resource/123"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE http://$HOST:$PORT/resource/123)
check_status "204" "$STATUS" "DELETE /resource/123"

# Test 503 Service Unavailable (Timeout simulé)
echo -e ${YELLOW}"TEST 503 Service Unavailable: http://$HOST:$PORT/slow-endpoint"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" --max-time 1 http://$HOST:$PORT/slow-endpoint)
check_status "503" "$STATUS" "GET /slow-endpoint (timeout)"

# Test 408 Request Timeout
echo -e ${YELLOW}"TEST 408 Request Timeout: http://$HOST:$PORT/very-slow"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" --max-time 1 http://$HOST:$PORT/very-slow)
check_status "408" "$STATUS" "GET /very-slow (timeout)"

# Test 200 OK avec Content-Type JSON
echo -e ${YELLOW}"TEST 200 OK + JSON: http://$HOST:$PORT/api/data"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -H "Accept: application/json" http://$HOST:$PORT/api/data)
check_status "200" "$STATUS" "GET /api/data (JSON)"

# Test 415 Unsupported Media Type
echo -e ${YELLOW}"TEST 415 Unsupported Media Type: POST http://$HOST:$PORT/api"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X POST -H "Content-Type: text/xml" -d '<data>test</data>' http://$HOST:$PORT/api)
check_status "415" "$STATUS" "POST /api (XML non supporté)"

# Test 401 Unauthorized (Auth manquante)
echo -e ${YELLOW}"TEST 401 Unauthorized: http://$HOST:$PORT/secure"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:$PORT/secure)
check_status "401" "$STATUS" "GET /secure (sans auth)"

# Test 403 Forbidden (Auth insuffisante)
echo -e ${YELLOW}"TEST 403 Forbidden: http://$HOST:$PORT/admin"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -u user:wrongpass http://$HOST:$PORT/admin)
check_status "403" "$STATUS" "GET /admin (mauvais mot de passe)"

# Test 413 Payload Too Large
echo -e ${YELLOW}"TEST 413 Payload Too Large: POST http://$HOST:$PORT/upload"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X POST --data-binary @large_file.bin http://$HOST:$PORT/upload)
check_status "413" "$STATUS" "POST /upload (fichier trop gros)"

# Test CORS (Preflight OPTIONS)
echo -e ${YELLOW}"TEST CORS Preflight: OPTIONS http://$HOST:$PORT/api"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -X OPTIONS -H "Origin: http://autre-domaine.com" -H "Access-Control-Request-Method: POST" http://$HOST:$PORT/api)
check_status "200" "$STATUS" "OPTIONS /api (CORS)"

# Test 304 Not Modified (Cache)
echo -e ${YELLOW}"TEST 304 Not Modified: http://$HOST:$PORT/cached"${DEF}
STATUS=$(curl -s -o /dev/null -w "%{http_code}" -H "If-Modified-Since: Wed, 21 Oct 2025 07:28:00 GMT" http://$HOST:$PORT/cached)
check_status "304" "$STATUS" "GET /cached (If-Modified-Since)"
