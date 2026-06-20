// ===== Gestion du thème clair/sombre (basée sur le cookie "theme") =====
// A inclure sur TOUTES les pages, juste avant la fin du <body> :
//   <script src="scripts/theme.js"></script>
// Et avoir un bouton sur la page avec ce code (l'id et le onclick sont importants) :
//   <button type="button" class="btn-theme" id="btn-theme" onclick="toggleDarkMode()">Activer le Mode Sombre</button>
 
function setCookie(nom, valeur, jours) {
    let date = new Date();
    date.setTime(date.getTime() + (jours * 24 * 60 * 60 * 1000));
    let expires = "expires=" + date.toUTCString();
    document.cookie = nom + "=" + valeur + ";" + expires + ";path=/";
}
 
function getCookie(nom) {
    let nomRecherche = nom + "=";
    let cookies = document.cookie.split(';');
    for (let i = 0; i < cookies.length; i++) {
        let c = cookies[i].trim();
        if (c.indexOf(nomRecherche) === 0) {
            return c.substring(nomRecherche.length, c.length);
        }
    }
    return "";
}
 
function toggleDarkMode() {
    let body = document.body;
    let btn = document.getElementById("btn-theme");
 
    if (body.classList.contains("dark-mode")) {
        body.classList.remove("dark-mode");
        setCookie("theme", "light", 30);
        if (btn) btn.innerText = "Activer le Mode Sombre";
    } else {
        body.classList.add("dark-mode");
        setCookie("theme", "dark", 30);
        if (btn) btn.innerText = "Désactiver le Mode Sombre";
    }
}
 
// Applique automatiquement le thème mémorisé dès que N'IMPORTE QUELLE
// page contenant ce script se charge.
document.addEventListener("DOMContentLoaded", function () {
    if (getCookie("theme") === "dark") {
        document.body.classList.add("dark-mode");
        let btn = document.getElementById("btn-theme");
        if (btn) btn.innerText = "Désactiver le Mode Sombre";
    }
});

