/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventsLoop.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchallat <bchallat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:12:23 by bchallat          #+#    #+#             */
/*   Updated: 2026/02/27 14:15:10 by bchallat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTSLOOP_HPP
#define EVENTSLOOP_HPP

// ================================== LOOP ================================== //

class EventsLoop: public Clients {

  private :
    EventsLoop( void );
    virtual ~EventsLoop( void );
    EventsLoop(const EventsLoop& other);
    EventsLoop& operator=(const EventsLoop& other);
};

#endif

/* ************************************************************************** */
/*                                                                            */
/* ************************************************************************** */


