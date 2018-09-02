const express = require('express')
const app = express()

const bodyParser = require("body-parser");

/** bodyParser.urlencoded(options)
 * Parses the text as URL encoded data (which is how browsers tend to send form data from regular forms set to POST)
 * and exposes the resulting object (containing the keys and values) on req.body
 */
app.use(bodyParser.urlencoded({
    extended: true
}));


app.put('/', function(req, res) {
	      //console.log('POST: El servidor ha recibido algo:' +req.body);
    if(req.body != null && req.body.ID != null){

        //Hemos obtenido el mensaje del arduino y lo tenemos almacenado en: req.query.msg
        
        //res.send('ID: '+req.body.ID+ ' Options: '+req.body.options);   
		console.log('{position:'+req.body.position+',Personas detectadas:'+req.body.detectedPeople+'}');
    } else res.send('No se ha especificado el parámetro PUT') ;
});

app.listen(3000, () => console.log('Servidor en puerto 3000.'))

