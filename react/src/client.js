var db = require('./dbcon.js');
var indeed = db.getPool();

var express    = require('express');
var app        = express();
var bodyParser = require('body-parser');

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

var port = process.env.PORT || 8080;

indeed.query('SELECT * FROM indeed', (err, res) => {
  console.log(err, res);
  indeed.end();
})

var router = express.Router();
router.get('/', function(req, res) {
    indeed.connect();
    res.json({ message: 'hooray! welcome to our api!' });
});

// more routes for our API will happen here

app.use('/', router);

// START THE SERVER
// =============================================================================
app.listen(port);
console.log('Magic happens on port ' + port);
