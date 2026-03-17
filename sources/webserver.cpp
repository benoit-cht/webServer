/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:24:16 by bchallat          #+#    #+#             */
/*   Updated: 2026/03/17 14:14:30 by ton_utilisate    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/webserver.hpp"
//# include "../include/SocketTcp.hpp"

//int main (int av, char *ag[])
int main()
{
  Socket soket;
  EventsLoop event(soket);
  //while (true)
  //{
    event.acceptNewConnection();
 // }
    return 0;
}

/*

    while (true) {
        // 1. Attendre une activité sur les sockets
        int ret = poll(&_poll_fds[0], _poll_fds.size(), -1);  // Timeout infini
        if (ret == -1) {
            perror("poll() failed");
            break;
        }

        // 2. Vérifier si une nouvelle connexion arrive
        if (_poll_fds[0].revents & POLLIN) {
            acceptNewConnection();
        }

        // 3. Traiter les requêtes des clients existants
        for (size_t i = 1; i < _poll_fds.size(); i++) {
            if (_poll_fds[i].fd != -1 && _poll_fds[i].revents & POLLIN) {
                handleClientRequest(_poll_fds[i].fd);
            }
        }
    }
*/
