/* name: EmployeeDAO.cs
 * description: this class has methods that allow access to the database
 */

using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Threading.Tasks;

namespace HelpdeskDAL
{
    public class EmployeeDAO
    {
        readonly IRepository<Employee> repository;
        public EmployeeDAO()
        {
            repository = new HelpdeskRepository<Employee>();
        }

        // Get employee by email
        public async Task<Employee> GetByEmail(string email)
        {
            Employee selectedEmployee;
            try
            {
                selectedEmployee = await repository.GetOne(emp => emp.Email == email);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return selectedEmployee;
        }

        // Get employee by id
        public async Task<Employee> GetById(int id)
        {
            Employee selectedEmployee;

            try
            {
                selectedEmployee = await repository.GetOne(emp => emp.Id == id);

            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return selectedEmployee;
        }

        // Get all employees
        public async Task<List<Employee>> GetAll()
        {
            List<Employee> selectedEmployees = new List<Employee>();
            try
            {
                selectedEmployees = await repository.GetAll();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return selectedEmployees;
        }

        // Add employee
        public async Task<int> Add(Employee newEmployee)
        {
            try
            {
                await repository.Add(newEmployee);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return newEmployee.Id;
        }

        //Update employee
        public async Task<UpdateStatus> Update(Employee updatedEmployee)
        {
            UpdateStatus operationStatus = UpdateStatus.Failed;
            try
            {
                operationStatus = await repository.Update(updatedEmployee);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return operationStatus;
        }

        // Delete employee using id
        public async Task<int> Delete(int id)
        {
            int employeeDeleted = -1;
            try
            {
                await repository.Delete(id);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return employeeDeleted;
        }

    }
}
