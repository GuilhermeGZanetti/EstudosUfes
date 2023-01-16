'use strict';

const express = require('express');
const bodyParser = require('body-parser')
const app = express()
const db = require('./queries.js');

app.use(bodyParser.json())
app.use(
  bodyParser.urlencoded({
    extended: true,
  })
)


// Constants
const port = 8080;


app.get('/', (request, response) => {
    response.json({ info: 'Node.js, Express, and Postgres API' })
})

app.get('/usertable', db.getUsers)


app.listen(port, () => {
    console.log(`App running on port ${port}.`)
})

