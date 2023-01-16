const Pool = require('pg').Pool
const pool = new Pool({
  user: 'bocauser',
  host: 'boca-postgres',
  database: 'bocadb',
  password: 'dAm0HAiC',
  port: 5432,
})
const getUsers = (request, response) => {
  pool.query('SELECT * FROM usertable', (error, results) => {
    if (error) {
      throw error
    }
    response.status(200).json(results.rows)
  })
}


module.exports = {
  getUsers,
}