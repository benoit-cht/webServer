`curl -X POST http://localhost:4242/ -F "file=@/home/tjacquel/Documents/web-surf/test.txt"`
Resultat attendu: valeur de retour attendue: 201
N.B: il faut que test.txt existe sinon on aura un 403 (et c'est normal)

```
nc -C localhost 4242
POST / HTTP/1.1
Host: 127.0.0.1:4242
Transfer-Encoding: chunked
Content-Type: text/plain

4
Wiki
5
pedia
0
```
Resultat attendu: 201?
