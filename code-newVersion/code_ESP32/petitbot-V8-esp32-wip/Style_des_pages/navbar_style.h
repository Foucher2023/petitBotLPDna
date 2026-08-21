#ifndef NAVBAR_STYLE_H
#define NAVBAR_STYLE_H

const char NAVBAR_CSS[] PROGMEM = R"rawliteral(
/* ===== Variables CSS ===== */
:root {
  --color-bg: #222;
  --color-text: #fff;
  --color-text-secondary: #ddd;
  --color-active: #00adb5;
  --color-hover: #333;
  --color-border: #333;
  --font-family: Arial, sans-serif;
  --font-size-brand: 5vw;
  --font-size-brand-desktop: 2.5vw;
  --font-size-nav: 4.5vw;
  --font-size-nav-desktop: 2vw;
  --padding-header: 3vw 4vw;
  --padding-header-desktop: 1vw 2vw;
  --padding-nav: 4vw;
  --padding-nav-desktop: 2vw;
  --icon-bar-width: 6vw;
  --icon-bar-height: 0.6vw;
  --icon-bar-margin: 1vw 0;
  --icon-bar-radius: 1vw;
  --transition: all 0.2s ease;
}

/* ===== Navbar ===== */
.navbar {
  width: 100%;
  background-color: var(--color-bg);
  color: var(--color-text);
  font-family: var(--font-family);
}

/* ===== Conteneur ===== */
.container-fluid {
  display: flex;
  flex-direction: column;
}

/* ===== En-tête ===== */
.navbar-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: var(--padding-header);
}

/* Marque (nom du site) */
.navbar-brand {
  color: var(--color-text);
  text-decoration: none;
  font-size: var(--font-size-brand);
  font-weight: bold;
}

/* ===== Bouton Hamburger ===== */
.navbar-toggle {
  background: none;
  border: none;
  cursor: pointer;
  padding: 0;
  margin: 0;
}

.icon-bar {
  display: block;
  width: var(--icon-bar-width);
  height: var(--icon-bar-height);
  background-color: var(--color-text);
  margin: var(--icon-bar-margin);
  border-radius: var(--icon-bar-radius);
}

/* ===== Menu Navigation ===== */
.navbar-collapse {
  display: none;
  background-color: var(--color-bg);
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
  border-top: 1px solid var(--color-border);
}

.nav li a {
  display: block;
  padding: var(--padding-nav);
  color: var(--color-text-secondary);
  text-decoration: none;
  font-size: var(--font-size-nav);
  transition: var(--transition);
}

.nav li a:hover {
  background-color: var(--color-hover);
  color: var(--color-text);
}

/* Lien actif */
.nav li.active a {
  background-color: var(--color-active);
  color: var(--color-text);
}

/* ===== Desktop (écrans larges) ===== */
@media (min-width: 768px) {
  .container-fluid {
    flex-direction: row;
    align-items: center;
    justify-content: space-between;
  }

  .navbar-header {
    padding: var(--padding-header-desktop);
  }

  .navbar-toggle {
    display: none;
  }

  .navbar-collapse {
    display: flex !important;
    background-color: transparent;
  }

  .nav {
    display: flex;
    margin-left: auto;
  }

  .nav li {
    border: none;
  }

  .nav li a {
    padding: var(--padding-nav-desktop);
    font-size: var(--font-size-nav-desktop);
  }

  .navbar-brand {
    font-size: var(--font-size-brand-desktop);
  }
}
)rawliteral";
#endif
