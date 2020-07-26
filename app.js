const express = require("express");
const path = require("path");
const ejs = require("ejs");
const bodyParser = require("body-parser");
const {PythonShell} = require("python-shell");
const execFile = require('child_process').execFile
const app = express();
const viewPath = path.join(__dirname, "/views");

app.use(express.static("public"));
app.set("view engine", "ejs");
app.set("views", viewPath);
app.use(bodyParser.urlencoded({
  extended: true
}));

// usage
// node-gyp configure build
// node app.js
////////////////////////////////// Web Routes //////////////////////////////////

app.get("/", function(req, res) {
  res.render("demo", {flag: false, err: false, M_val: 0, P_val: 3, Q_val: 3});
});

app.post("/", function(req, res) {
  const P_val = req.body.P;
  const Q_val = req.body.Q;
  const M_val = req.body.M;
  if(M_val < 0 || M_val > P_val*Q_val*P_val*Q_val) {
    res.render("demo", {
      flag: false,
      err: true,
      message: "M must be [0, PxQxPxQ]",
      M_val: M_val,
      P_val: P_val,
      Q_val: Q_val
    });
  }
  const MRV = req.body.MRV;
  const MAD = req.body.MAD;
  const LCV = req.body.LCV;
  const FC = req.body.FC;
  const NOR = req.body.NOR;
  let options = {
      args: ["board", 1, P_val, Q_val, M_val]
  };
  PythonShell.run('Board_Generator.py', options, function (err, results) {
      if (err) throw err;
      // results is an array consisting of messages collected during execution
      //console.log('results: %j', results);
      var program = "build/Release/Sudoku";
      var child = execFile(program, [
        (MRV)?"MRV":"",
        (MAD)?"MAD":"",
        (LCV)?"LCV":"",
        (FC)?"FC":"",
        (NOR)?"NOR":"",
        "board_0.txt"
      ], function(error, stdout, stderr) {
        var result = stdout.split("|");
        var initNumbers = result[0].substring(0, P_val*P_val*Q_val*Q_val);
        var initBoard = [];
        var pos = 0;
        for(var i=0; i<P_val*Q_val; i++) {
          var row = [];
          for(var j=0; j<P_val*Q_val; j++) {
            row.push(initNumbers.charAt(pos++));
          }
          initBoard.push(row);
        }
        var failed = false;
        var resNumbers = "";
        var resBoard = [];
        var push = 0;
        var back = 0;
        var time_elapsed = 0;
        if(result[0].charAt(P_val*P_val*Q_val*Q_val+1) === 'F') {
          failed = true;
        } else {
          resNumbers = result[0].substring(P_val*P_val*Q_val*Q_val+1, 2*P_val*P_val*Q_val*Q_val+1);
          resBoard = [];
          pos = 0;
          for(var i=0; i<P_val*Q_val; i++) {
            var row = [];
            for(var j=0; j<P_val*Q_val; j++) {
              row.push(resNumbers.charAt(pos++));
            }
            resBoard.push(row);
          }
          push = result[1];
          back = result[2];
          time_elapsed = result[3];
        }
        res.render("demo", {
          flag: true,
          failed: failed,
          P_val: P_val,
          Q_val: Q_val,
          initBoard: initBoard,
          resBoard: resBoard,
          pushes: push,
          backtracks: back,
          time: time_elapsed
        });
        res.end();
      });
  });
});

////////////////////////////////// Port Connection Section //////////////////////////////////

let port = process.env.PORT;
if (port == null || port == "") {
  port = 3000;
}

app.listen(port, function() {
  console.log("Server started on port " + port + " successfully!");
});
