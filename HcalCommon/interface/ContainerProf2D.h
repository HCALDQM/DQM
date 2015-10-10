#ifndef ContainerProf2D_h
#define ContainerProf2D_h

/*
 *	file:		ContainerProf2D.h
 *	Author:		Viktor Khristenko
 *
 *	Description:
 *		Container to hold TProfile or like
 *
 */

#include "DQM/HcalCommon/interface/Container2D.h"

#include <vector>
#include <string>

namespace hcaldqm
{
	class ContainerProf2D : public Container2D
	{
		public:
			ContainerProf2D() {}
			virtual ~ContainerProf2D() {}

			virtual bool book(DQMStore::IBooker&);
			virtual void book(DQMStore::IBooker&);

		protected:
	};
}


#endif








