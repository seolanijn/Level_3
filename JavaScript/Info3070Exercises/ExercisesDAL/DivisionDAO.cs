using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Threading.Tasks;


namespace ExercisesDAL
{
    public class DivisionDAO
    {
        readonly IRepository<Division> repository;
        public DivisionDAO()
        {
            repository = new SomeScholRepository<Division>();
        }
        public async Task<List<Division>> GetAll()
        {
            List<Division> allDivisions = new List<Division>();
            try
            {
                allDivisions = await repository.GetAll();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return allDivisions;
        }
    }
}
