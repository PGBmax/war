Todo list:

Manda:
Edit signature of each file infected
add fingerprint to personal signature

Bonus:
Add more commands


Commands :
```Filesystem

Commande	Description
cat <file>	Affiche le contenu d'un fichier (via read() en boucle)
head <n> <file> / tail <n> <file>	Affiche les N premières/dernières lignes
stat <file>	Infos sur un fichier (taille, perms, timestamps via fstat)
mkdir <dir>	Crée un répertoire (mkdir())
cp <src> <dst>	Copie un fichier
mv <src> <dst>	Déplace/renomme (rename())
chmod <octal> <file>	Change les permissions (chmod())
find <path> <pattern>	Recherche récursive de fichiers
Exécution

Commande	Description
exec <cmd>	Exécute une commande shell et renvoie stdout/stderr (via popen)
shell	Spawn un sh interactif attaché au fd client (comme ton run_bind_shell mais intégré dans le serveur existant)
Système / Info

Commande	Description
whoami	Affiche l'UID/username courant (getuid() + getpwuid())
ps	Liste les processus (lecture de /proc/*/status)
kill <pid>	Kill un processus (kill())
sysinfo	Hostname, uptime, mémoire disponible (meminfo, uname())
env	Affiche les variables d'environnement (via environ)
setenv <key> <val>	Définit une variable d'env (setenv())
Réseau

Commande	Description
netstat	Connexions TCP actives (lecture de tcp)
download <file>	Envoie le contenu d'un fichier au client via la socket (tu as déjà receiving_file/file_size dans t_client, semble prévu)
Persistance / Virus

Commande	Description
persist	Relance setup_service_file() pour re-s'installer
spread <dir>	Déclenche manuellement l'infection sur un répertoire
infect <binary>	Infecte un binaire spécifique```