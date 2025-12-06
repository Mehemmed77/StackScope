import http from 'http';
import { readFile } from 'fs/promises';

const server = http.createServer(async (request, response) => {
    if (request.method === "GET") {
        const url = request.url;
        if (url.includes("?")) {
            const index = url.indexOf("?");
            
            
        }
    }

    response.writeHead(200, { "Content-type": "text/plain" });
    response.end("aa");
});


server.listen(3000, () => {
    console.log("Server started on http://localhost:3000");
});
