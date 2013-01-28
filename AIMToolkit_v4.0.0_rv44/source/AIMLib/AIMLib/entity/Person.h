/*
Copyright (c) 2008-2013, Northwestern University
All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 
  Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
 
  Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
 
  Neither the name of the Northwestern University nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#ifndef _AIMLib_Person_Class_
#define _AIMLib_Person_Class_

#include <string>
#include <ctime>

namespace aim_lib
{
	class AIMLIB_API Person
	{
	public:
		Person();
		Person(const Person& person);
		virtual ~Person(void);
		Person* Clone() const;

		const std::string& GetId() const;
		const std::string& GetName() const;
		const Date& GetBirthDate() const;
		const std::string& GetSex() const;
		const std::string& GetEthnicGroup() const;
		void SetId(const std::string& newVal);
		void SetName(const std::string& newVal);
		void SetBirthDate(const Date& newVal);
		void SetSex(const std::string& newVal);
		void SetEthnicGroup(const std::string& newVal);


	private:
		std::string _id;
		std::string _name;
		Date _birthDate;
		std::string _sex;
		std::string _ethnicGroup;
	};
}

#endif // _AIMLib_Person_Class_