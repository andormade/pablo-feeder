const express = require('express')
const ip6addr = require('ip6addr')
const app = express()
const port = process.env.PORT || 6969
let address = '';
let lastUpdate = 0;

app.get('/update-address', (req, res) => {
	const {port, protocol} = req.query

	if (!port || !protocol) {
		return res.sendStatus(400)
	}

	const host = ip6addr.parse(req.connection.remoteAddress).toString({ format: 'v4' })
	address = protocol + '://' + host + ':' + port
	lastUpdate = Date.now()
	res.send(req.header('x-forwarded-for'))
	//res.sendStatus(200)
})

app.get('/get-address', (req, res) => {
	res.json({ address, lastUpdate })
})

app.listen(port, () => console.log(`Listening on ${port}...`))