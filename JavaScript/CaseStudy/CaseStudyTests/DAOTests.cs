using System;
using Xunit;
using HelpdeskDAL;
using System.Linq;
using System.Diagnostics;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace CaseStudyTests
{
    public class DAOTests
    {
        [Fact]
        public async Task Employee_GetByEmailTest()
        {
            EmployeeDAO dao = new EmployeeDAO();
            Employee selectedEmployee = await dao.GetByEmail("bs@abc.com");
            Assert.NotNull(selectedEmployee);
        }
        [Fact]
        public async Task Employee_GetByIdTest()
        {
            EmployeeDAO dao = new EmployeeDAO();
            Employee selectedEmployee = await dao.GetById(1);
            Assert.NotNull(selectedEmployee);
        }
        [Fact]
        public async Task Employee_GetAllTest()
        {
            EmployeeDAO dao = new EmployeeDAO();
            List<Employee> selectedEmployee = await dao.GetAll();
            Assert.NotNull(selectedEmployee);
        }
        [Fact]
        public async Task Employee_AddTest()
        {
            EmployeeDAO dao = new EmployeeDAO();
            Employee newEmployee = new Employee
            {
                Title = "Ms.",
                FirstName = "Seolan",
                LastName = "Jin",
                Email = "sj@somecompany.com",
                PhoneNo = "(555)555-5551",
                DepartmentId = 100
            };
            await dao.Add(newEmployee);
            Assert.NotNull(newEmployee);
        }
        [Fact]
        public async Task Employee_UpdateTest()
        {
            EmployeeDAO dao = new EmployeeDAO();
            Employee selectedEmployee = await dao.GetByEmail("sj@somecompany.com");
            if (selectedEmployee != null)
            {
                string oldPhoneNo = selectedEmployee.PhoneNo;
                string newPhoneNo = oldPhoneNo == "(555)555-5551" ? "(555)555-1251" : "(555)555-5551";
                selectedEmployee.PhoneNo = newPhoneNo;
            }
            Assert.True(await dao.Update(selectedEmployee) == UpdateStatus.Ok);
        }
        [Fact]
        public async Task Employee_DeleteTest()
        {
            EmployeeDAO dao = new EmployeeDAO();
            await dao.Delete(12);
        }
        [Fact]
        public async Task Employee_ConcurrencyTest()
        {
            EmployeeDAO dao1 = new EmployeeDAO();
            EmployeeDAO dao2 = new EmployeeDAO();
            Employee employeeForUpdate1 = await dao1.GetByEmail("sj@somecompany.com");
            Employee employeeForUpdate2 = await dao2.GetByEmail("sj@somecompany.com");

            if (employeeForUpdate1 != null)
            {
                string oldPhoneNo = employeeForUpdate1.PhoneNo;
                string newPhoneNo = oldPhoneNo == "519-555-1234" ? "555-555-5555" : "519-555-1234";
                employeeForUpdate1.PhoneNo = newPhoneNo;
                if (await dao1.Update(employeeForUpdate1) == UpdateStatus.Ok)
                {
                    employeeForUpdate2.PhoneNo = "666-666-6666";
                    Assert.True(await dao2.Update(employeeForUpdate2) == UpdateStatus.Stale);
                }
                else
                    Assert.True(false);
            }
        }

        [Fact]
        public async Task Employee_LoadPicsTest()
        {
            try
            {
                DALUtil util = new DALUtil();
                Assert.True(await util.AddEmployeePicsToDb());
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

    }
}
