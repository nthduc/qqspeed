clientserverinfo()
print(">>>> Start Angell.lua >>>>")

--JZ: load level
level_load_physics("level/Angell/Angell_ps.nif", "NAV_GROUND", "NAV_AIR", "COLLISION", "SECTION")
level_load_logic("level/Angell/Angell_lg.nif")
if clientgame() then
      level_load_visual("level/Angell/changjing.nif")
	  --level_load_visual("level/Angell/Angell_A.nif")
      --level_load_visual("level/Angell/Angell_B.nif")
      --level_load_visual("level/Angell/Angell_C.nif")
      --level_load_visual("level/Angell/Angell_D.nif")
	  --level_load_skydoom("level/Angell/skydome.nif")
end

--JZ: load level
--[[level_load_physics("level/test/treeroottown_ps.nif", "NAV_GROUND", "NAV_AIR", "COLLISION", "SECTION")
level_load_logic("level/test/treeroottown_lg.nif")
if clientgame() then
	level_load_visual("level/test/treeroottown_vs.nif")
	level_load_skydoom("level/test/treeroottown_skydome.nif")
end
]]
function load_data (datafile)
		dofile(datafile)
		--read all data
		local target_table = {}
		local target_title = {}
		local title_len = 0
		local temp_data_line = {}
		local iter = 1
		local line_iter = 1
		local data_tag = 0

		while true do
			if datafile[iter] then
				temp_data_line[line_iter] = datafile[iter]
				
				--print(temp_data_line[line_iter])
				line_iter = line_iter +1
				iter = iter + 1
				data_tag = 0
			else
				data_tag = data_tag +1
			
				if data_tag == 1 then
					if target_title[1] == nil then
						title_len = line_iter - 1
						for i = 1,title_len do
							target_title[i] = temp_data_line[i]
						end
					else
						if title_len ~= line_iter - 1 then
							print("error at "..iter)
						else
							for i = 1,title_len do
								local title = target_title[i]
								
								target_table[title] = temp_data_line[i]
							end
						end
					end
				
					line_iter = 1
					iter = iter + 1
					
				else
					break
				end
			end
		end			
		--check data
		for i,v in target_table do
			if type(v) == "table" then
				for j,w in v do
					print(w)
				end
			else
				print(v)
			end
		end
		
		
		return target_table
end

whole_monster_data = load_data("Level/Angell/common_data.lua")
function monster_add (common_index, is_single)
	if servergame() then                
		local monster_data = whole_monster_data[common_index]
                local group_number = monster_data.Num
                if is_single then group_number = 1 end
                for n = 1,  group_number do
                      local monster_id = spawn("scripts/"..monster_data.UseLua)
                      level.monster_index[monster_id] = common_index
                      local pos =monster_data.Position
                      set_bornpoint(monster_id, pos[1] ,pos[2], pos[3])
                      set_position(monster_id,pos[1] ,pos[2], pos[3])
                      set_enemy_center(monster_id,pos[1] ,pos[2], pos[3])
                      
                      for i,v in monster_data.MainEnemyList do
                              add_enemy(monster_id,v,true)
                      end
                      for i,v in monster_data.SubEnemyList do
                              add_enemy(monster_id,v,false)
                      end
                      
                      setairadius(monster_id,monster_data.RoamRange)
                      
                      --复活时间设定
                      level.rise_time[monster_id]=monster_data.RespawnTime		
                      --设置攻击权重
                      local weight = monster_data.EnemyWeight
                      set_weight(monster_id,weight[1],weight[2],weight[3])
                      --设置npc追击半径
                      local range = get_rand_between(monster_data.ChaseRange[1],monster_data.ChaseRange[2])
                      set_chase_radius(monster_id,range)
                      --设定当生命值下降到多少白分比时逃跑
                      local percent = get_rand_between(monster_data.PercentOfRun[1],monster_data.PercentOfRun[2])/100
                      set_runaway_percentage(monster_id,percent)
                      
                      if monster_data.Role.common then
                            set_name(monster_id,"common_monster")
                      elseif monster_data.Role.jail then	
                              setairadius(monster_id,5)	--a fix number currently
                              set_weight(monster_id,0,0,0)
                              
                              local t = monster_data.SystemID
                              level.jail_system[t].jail_count = level.jail_system[t].jail_count + 1
                              level.jail_actor.system_id[monster_id] = t
                              level.jail_actor.retort_pos[monster_id]=monster_data.TargetNode
                              
                              set_name(monster_id, "jail_actor")
                      elseif monster_data.Role.guard then
                              local t = monster_data.SystemID
                              level.jail_system[t].guard_count = level.jail_system[t].guard_count + 1
                              level.guard_actor.system_id[monster_id] = t
                              
                              set_name(guard_katydid, "guard_actor")
                      elseif monster_data.Role.teammate then
                              local t = monster_data.SystemID
                              
                              level.team[t].id_pool = {}
                              level.teammate.team_id[monster_id] = t
                              level.team[t].id_pool[n] = monster_id
                              level.team[t].member_count = level.team[t].member_count+1
                              set_name(guard_katydid, "teammate")
                      elseif monster_data.Role.relate then
                              activate_ai(monster_id,false)	 
                               
                              level.team[monster_data.SystemID].related_target = monster_id
                              set_name(monster_id,"common_monster")
                      end
                      init_level_event(monster_id)
                end
	end   
end

function system_add(system_id)
        for i,v in whole_monster_data do
              if v.SystemID == system_id then
                    monster_add(v.ID)
              end
        end
end

---------------------------------------------------------------------------------------------

level = {}
level.controller = {
	on_init = function(self)
	  print("level.on_init")
		if servergame() then
			-- setup level update
			register_level_event(self, "ev_on_update")
			set_level_update(self, 2.0)
                        
                        level.monster_index={}
			level.rise_time={}
			--for jail system
			level.guard_actor.system_id = {}
			level.jail_actor.system_id = {}
			level.jail_system = {}
                        level.jail_actor.retort_pos={}
                        
			level.teammate.team_id = {}
			level.team = {}
		end
      end,
      ev_on_update = function(self)      
      --print("level update")
      end,
}


level.common_monster =
{
 on_init = function(self)
       --print("katydid_2 init")
       if servergame() then
                   --register_level_event(self, "ev_on_die")
		   register_level_event(self, "ev_on_update")
		   set_level_update(self, 1.0)
       end
  end,
  ev_on_update = function(self)
  
		if servergame()	then
			update_enemy_center(self);
                        local cur_ac =	getfsmstatename(self,Actor.fsm_action)
			if cur_ac=="action_dead" then
				if level.rise_time[self]<=0 then
				  print("rise_time_0",level.rise_time[self])
				  destroy(self) 
                                  local my_index = level.monster_index[self]
                                  level.monster_index[self] = nil
                                  level.rise_time[self] = nil
				  monster_add(my_index,true)  
				else
				  print("rise_time",level.rise_time[self])
				  level.rise_time[self]=level.rise_time[self]-1
				end
			end
		end
  end,		
  ev_on_die = function(self)      
        --  print("katydid_2 die") 
  end,	
}

level.teammate = 
{
	on_init = function(self)
		if servergame() then
			-- setup level update event, this causes the on_update() being called every 1.0 s
			register_level_event(self, "ev_on_die")
			register_level_event(self, "ev_on_update")
			set_level_update(self, 1.0)
		end
	end,
	
	ev_on_update = function(self)
	--print("big_katydid update")
	--get team id		
		--update_enemy_center(self)
		
		local my_team = level.teammate.team_id[self]
		
		local minlife = 0
		local tempid = 0
		--local enemy = 0
		
		for i,v in level.team[my_team].id_pool do
			--print(v)
			local life = get_cur_life(v)
			if i == 1 or minlife>life then
				minlife = life
				tempid = v
			end
		end
		--now tempid is the actor with the least life in team
		--check if tempid is me
		if tempid == self then
			--print("tempid"..tempid)
			local attacker = getattackerid(tempid)
			if attacker ~= 0 then
				--print("attacker"..attacker)
				for i,v in level.team[my_team].id_pool do	
					if target_is_close(v,attacker,get_chase_radius(self)) then
						--print("who"..v)
						level.team[my_team].enemy = attacker
					end
				end
			end
		end		
		
		if level.team[my_team].enemy == 0 then
			--attacker not here
			--search other enemy
			local cur_st = getfsmstatename(self,Actor.fsm_action)
			if cur_st ~= "action_idle" then --will change when add new action state
				local check = get_enemy(self)
				if check ~= 0 then
					level.team[my_team].enemy = check
				end
			end
		end
		
		local isclose = false
		for i,v in level.team[my_team].id_pool do	
			if target_is_close(v,level.team[my_team].enemy,get_chase_radius(self)) then
				--print("isclose")
				isclose = true
			end
		end
		
		if not isclose then
			--print("not close"..self)
			level.team[my_team].enemy = 0
		end
		
		if level.team[my_team].enemy == 0 then
			--no enemy
			set_order(self,-1)
		else
			if tempid == self and get_cur_life(self)< getactorproperty(self).life * get_runaway_percentage(self) then
				--run away				
				sendtrigger(self,at_action_stop);
				set_order(self,ThinkGeneric.TS_Move)
			else
				--attack
				set_enemy(self,level.team[my_team].enemy)			
				set_order(self,ThinkGeneric.TS_Attack)
			end
		end	
			
		local cur_ac =	getfsmstatename(self,Actor.fsm_action)
		local my_team = level.teammate.team_id[self]
		
		if cur_ac=="action_dead" then
		    print("my_team",my_team,level.team[my_team].member_count,level.rise_time[self])
			if level.rise_time[self]<=0 and level.team[my_team].member_count == 0 then
				  print("rise_time_0",level.rise_time[self])
                                  
                                  local relate_pet = level.team[my_team].related_target
                                  local my_index = level.monster_index[self]
                                  local pet_index = level.monster_index[relate_pet] 
                                  --delete all info
				  for i,v in level.team[my_team].id_pool do
				       destroy(v) 
				       level.team[my_team].id_pool[i] = nil
				       level.teammate.team_id[v] = nil
                                       
                                       level.monster_index[v]  = nil
                                       level.rise_time[self] = nil
				  end
                                  level.monster_index[relate_pet] = nil
                                  level.rise_time[relate_pet] = nil
                                  level.team[my_team].related_target = nil
                                  
				  monster_add(my_index, false)
                                  monster_add(pet_index,false)
                                  
			else
				  --print("teammate rise_time",level.rise_time[self])
				  level.rise_time[self]=level.rise_time[self]-1
			end
		end
	end,
	
	ev_on_die = function(self)	
		--print("teammate died")
		local my_team = level.teammate.team_id[self]
		level.team[my_team].member_count = level.team[my_team].member_count - 1
  		if level.team[my_team].member_count == 0 then		
			local killer = getattackerid(self)
			update_pet_owner(level.team[my_team].related_target,killer,true)
                end
	end,
}

level.jail_actor = 
{	
	on_init = function(self)
	      if servergame() then
                      register_level_event(self, "ev_on_die")
                      register_level_event(self, "ev_on_update")
                      set_level_update(self, 1.0)
	      end
	end,
	ev_on_update = function(self)
	--print("jail_katydid update")
		update_enemy_center(self)
		local my_system = level.jail_actor.system_id[self]
		--print("system jail"..my_system..self)
		if level.jail_system[my_system].guard_count == 0 and level.jail_actor.retort_pos[self]~=nil then
		     print("jail_actor",self,level.jail_actor.retort_pos[self])
		     local pos =get_level_node_position(level.jail_actor.retort_pos[self])
			 set_bornpoint(self, pos.x, pos.y, pos.z)
                         local my_index = level.monster_index[self]                         
                         local my_data = whole_monster_data[my_index]
                         
                        setairadius(self,my_data.RoamRange)
                        local weight = my_data.EnemyWeight
                        set_weight(self,weight[1],weight[2],weight[3])
                        
			 level.jail_actor.retort_pos[self]=nil
		end	
		
		if level.rise_time[self]<=0 and level.jail_system[my_system].jail_count == 0 then
			 print("rise_time_0",level.rise_time[self])
                                                 
                        level.jail_actor.system_id[self] = nil
                        level.rise_time[self] = nil
                        level.monster_index[monster_id]  = nil
                        destroy(self)
                         
                         system_add(my_system)
                    else
		     --print("teammate rise_time",level.rise_time[self])
                        level.rise_time[self]=level.rise_time[self]-1
                    end
	end,
	ev_on_die = function(self)	
	--print("jail_actor died")
		local my_system = level.jail_actor.system_id[self]
		level.jail_system[my_system].jail_count = level.jail_system[my_system].jail_count - 1
		if level.jail_system[my_system].jail_count == 0 then
                      --leave last one to calculate time
                      return
		end
		level.jail_actor.system_id[self] = nil
                level.rise_time[self] = nil
                level.monster_index[monster_id]  = nil
		destroy(self) 
	end,
}


--看守系统
level.guard_actor = 
{	
	on_init = function(self)
                if servergame() then
                        -- print(" guard_katydid on_init")
                        -- move to player
                        -- setup level update event, this causes the on_update() being called every 1.0 s
                        register_level_event(self, "ev_on_die")
                        register_level_event(self, "ev_on_update")
                        set_level_update(self, 1.0)
                end
	end,
	ev_on_update = function(self)
		--print("guard_katydid update")
		update_enemy_center(self)
		--local my_system = level.guard_actor.system_id[self]
		--print("system guard"..my_system..self)
	end,
	ev_on_die = function(self)	
	    --print("guard_katydid died")
		local my_system = level.guard_actor.system_id[self]
		print("my_system",my_system)
		level.guard_actor.system_id[self] = nil
		level.jail_system[my_system].guard_count = level.jail_system[my_system].guard_count - 1
                 level.rise_time[self] = nil
                level.monster_index[monster_id]  = nil
		destroy(self) 
	end,
}

print("<<<< Finish Angell.lua  <<<<")