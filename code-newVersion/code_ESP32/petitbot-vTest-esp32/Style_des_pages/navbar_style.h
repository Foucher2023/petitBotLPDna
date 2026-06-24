#ifndef NAVBAR_STYLE_H
#define NAVBAR_STYLE_H

const char NAVBAR_CSS[] PROGMEM = R"rawliteral(
/* ===== NAVBAR ===== */

.navbar {
  width: 100%;
  background-color: #222;
  color: #fff;
}

.navbar-inverse {
  background-color: #222;
}

.container-fluid {
  display: flex;
  flex-direction: column;
}

/* ===== HEADER ===== */

.navbar-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 3vw 4vw;
}

/* Brand */
.navbar-brand {
  color: #fff;
  text-decoration: none;
  font-size: 5vw;
  font-weight: bold;
}

/* ===== HAMBURGER BUTTON ===== */

.navbar-toggle {
  background: none;
  border: none;
  cursor: pointer;
}

.icon-bar {
  display: block;
  width: 6vw;
  height: 0.6vw;
  background-color: #fff;
  margin: 1vw 0;
  border-radius: 1vw;
}

/* ===== NAV LINKS ===== */

.navbar-collapse {
  display: none;
  background-color: #222;
}

.navbar-collapse.open {
  display: block;
}

.nav {
  list-style: none;
  padding: 0;
  margin: 0;
}

.nav li {
  border-top: 1px solid #333;
}

.nav li a {
  display: block;
  padding: 4vw;
  color: #ddd;
  text-decoration: none;
  font-size: 4.5vw;
}

.nav li a:hover {
  background-color: #333;
  color: #fff;
}

/* Active link */
.nav li.active a {
  background-color: #00adb5;
  color: #fff;
}

/* ===== DESKTOP ===== */
@media (min-width: 768px) {
  .container-fluid {
    flex-direction: row;
    align-items: center;
    justify-content: space-between;
  }

  .navbar-header {
    padding: 1vw 2vw;
  }

  .navbar-toggle {
    display: none;
  }

  .navbar-collapse {
    display: block !important;
  }

  .nav {
    display: flex;
  }

  .nav li {
    border: none;
  }

  .nav li a {
    padding: 2vw;
    font-size: 2vw;
  }

  .navbar-brand {
    font-size: 2.5vw;
  }
}
)rawliteral";

#endif
