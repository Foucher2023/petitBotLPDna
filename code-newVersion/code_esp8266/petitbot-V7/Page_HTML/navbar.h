#ifndef NAVBAR_H
#define NAVBAR_H

const char NAVBAR_HTML[] PROGMEM = R"rawliteral(
<link rel="stylesheet" href="/navbar.css">
<nav class="navbar navbar-inverse">
  <div class="container-fluid">
    <div class="navbar-header">
      <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#myNavbar">
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      <a class="navbar-brand" href="/">PetitBot</a>
    </div>
    <div class="collapse navbar-collapse" id="myNavbar">
      <ul class="nav navbar-nav">
        <li id="button-config"><a href="/config">Configuration</a></li>
        <li id="button-remote"><a href="/telecommande">Télécommande</a></li>
        <li id="button-programmation"><a href="/programmer">Programmer</a></li>
      </ul>
    </div>
  </div>
</nav>
)rawliteral";
#endif