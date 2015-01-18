
#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdexcept>

class SocketInput_ {

	static RH::Libs::EventDispatcher::EventPublicationManager eventPublicationManager;

	int socket_fd {-1};

	struct InputEvent {

		int cmd;
		int args[2];
	};

	struct State {

		InputEvent inputEvent;

		char * p;
		char * e;

		void Reset() {

			memset(&inputEvent, 0, sizeof inputEvent);
			p = (char*)&inputEvent;
			e = p+1;
		}

		State() {
			Reset();
		}

		const InputEvent & GetEvent() const {

			return inputEvent;
		}

		bool Update(int socket) {

			if(p<e) {

				ssize_t i = recvfrom(
					socket_fd,
					p, ((size_t)e)-((size_t)p),
					0, NULL, NULL);

				if(i>=0) {
					p+=i;
				}
				else {
					switch(errno) {
					case EWOULDBLOCK:
					case EAGAIN:
					case EINTR:
						break;
					default:
						throw std::runtime_error("input socket read error.");
					}
				}
			}

			return p==e;
		}
	};

	State state;

public:

	SocketInput_() {

		sockaddr_in addr;
		memset(&addr, 0, sizeof addr);

		addr.sin_family = AF_INET;
		addr.sin_port = htons( 8888 );
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if(( socket_fd = socket(AF_INET, SOCK_DGRAM, 0) ) == -1 )
			throw std::runtime_error("can't open input socket");

		if( bind(socket_fd, (const sockaddr*)(&addr), sizeof addr) == -1) {

			close(socket_fd);
			throw std::runtime_error("can't bind input socket");
		}

		if( fcntl(socket_fd, F_SETFL, O_NONBLOCK | fcntl(socket_fd, F_GETFL)) == -1) {
			close(socket_fd);
			throw std::runtime_error("can't set input socket to non-blocking.");
		}
	}

	virtual ~SocketInput_() {

		close(socket_fd);
	}

	void DispatchInputEvents() {

		while( state.Update( socket_fd) ) {

			const InputEvent & inputEvent = state.GetEvent();

			state.Reset();
		}
	}
};

