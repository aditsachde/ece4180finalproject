#pragma once

const char *const gethtml = R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=UTF-8

<!DOCTYPE html>
<html>
  <head>
    <title>AHRS Live Data</title>
  </head>
  <body>
    <h1>Real time heading</h1>
    <br />

    <h2>Roll</h2>
    <h2 id="roll"></h2>
    <br />

    <h2>Pitch</h2>
    <h2 id="pitch"></h2>
    <br />

    <h2>Yaw</h2>
    <h2 id="yaw"></h2>
    <br />

    <script>
      var refresh = window.setInterval(async () => {
        const response = await fetch("/", { method: "POST" });
        const responsetext = await response.text()
        console.log(responsetext)
        const split = responsetext.split(",");
        console.log(split);

        const roll = parseFloat(split[0]);
        const pitch = parseFloat(split[1]);
        const yaw = parseFloat(split[2]);

        document.getElementById("roll").textContent = roll;
        document.getElementById("pitch").textContent = pitch;
        document.getElementById("yaw").textContent = yaw;
      }, 500);
    </script>
  </body>
</html>
)";

const char *const postheader = R"(HTTP/1.1 200 OK
Content-Type: text/plain; charset=UTF-8

)";