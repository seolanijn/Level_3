using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Threading.Tasks;

namespace ExercisesDAL
{

    public class StudentDAO
    {
        readonly IRepository<Student> repository;
        public StudentDAO()
        {
            repository = new SomeScholRepository<Student>();
        }
        public async Task<Student> GetByLastname(string name)
        {
            Student selectedStudent;
            try
            {
                selectedStudent = await repository.GetOne(stu => stu.LastName == name);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return selectedStudent;
        }

        public async Task<Student> GetById(int id)
        {
            Student selectedStudent;

            try
            {
                selectedStudent = await repository.GetOne(stu => stu.Id == id);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }

            return selectedStudent;
        }

        public async Task<List<Student>> GetAll()
        {
            List<Student> allStudents = new List<Student>();
            try
            {
                allStudents = await repository.GetAll();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return allStudents;
        }

        public async Task<int> Add(Student newStudent)
        {
            try
            {
                await repository.Add(newStudent);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return newStudent.Id;
        }

        public async Task<UpdateStatus> Update(Student updatedStudent)
        {
            UpdateStatus operationStatus = UpdateStatus.Failed;
            try
            {
                operationStatus = await repository.Update(updatedStudent);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return operationStatus;
        }

        public async Task<int> Delete(int id)
        {
            int studentsDeleted = -1;

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
            return studentsDeleted;
        }

    }
}
